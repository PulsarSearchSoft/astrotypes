/*
 * MIT License
 * 
 * Copyright (c) 2018 PulsarSearchSoft
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

namespace pss {
namespace astrotypes {

View::View(MultiArrayType& array)
    : _container(array)
    , _fbegin(0)
    , _fend(0)
    , _sbegin(0)
    , _send(0)
{
}

View::~View()
{
}

typename View::Iterator View::begin()
{
    return Iterator();
}

typename View::ConstIterator View::begin() const
{
    return ConstIterator();
}

typename View::ConstIterator View::cbegin() const
{
    return ConstIterator();
}

typename View::Iterator View::end()
{
    return Iterator();
}

typename View::ConstIterator View::end() const
{
    return ConstIterator();
}

typename View::ConstIterator View::cend() const
{
    return ConstIterator();
}

} // namespace astrotypes
} // namespace pss
